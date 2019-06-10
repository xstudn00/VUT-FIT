using ICSapp.App.Commands;
using ICSapp.App.Views.Windows;
using ICSapp.BL.Messages;
using ICSapp.BL.Models;
using ICSapp.BL.Repositories;
using ICSapp.BL.Services;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;

namespace ICSapp.App.ViewModels
{
    public class TeamsListViewModel : BaseViewModel
    {

        private readonly IUserRepository userRepository;
        private readonly ITeamRepository teamRepository;
        private readonly IPostRepository postRepository;
        private readonly ICommentRepository commentRepository;


        public TeamModel Team { get; set; }
        public UserModel User { get; set; }
        public PostModel Post { get; set; }
        public List<TeamModel> TeamsList { get; set; }
        public List<CommentModel> CommentsList { get; set; }
        public List<PostModel> PostsList { get; set; }
        public CommentModel Comment { get; set; }

        public string infoText { get; set; }

        public ICommand DeleteTeamCommand { get; set; }
        public ICommand OpenTeamWindowCommand { get; set; }
        public ICommand DeleteCommentCommand { get; set; }
        public ICommand CreateCommentCommand { get; set; }
        public ICommand AddCommentCommand { get; set; }
        public ICommand DeletePostCommand { get; set; }
        public ICommand LoadPostCommand { get; set; }
        public ICommand CreatePostCommand { get; set; }
        public ICommand ActivateTeamCommand { get; set; }
        public ICommand CreateTeamCommand { get; set; }
        public ICommand RefreshTeamsWindowCommand { get; set; }

        private readonly IMediator mediator;

        public TeamsListViewModel(IUserRepository userRepository, IPostRepository postRepository, ITeamRepository teamRepository, ICommentRepository commentRepository, IMediator mediator)
        {
            this.userRepository = userRepository;
            this.postRepository = postRepository;
            this.teamRepository = teamRepository;
            this.commentRepository = commentRepository;

            this.mediator = mediator;
            User = new UserModel();
            Team = new TeamModel();
            Post = new PostModel();
            Comment = new CommentModel();

            OpenTeamWindowCommand = new RelayCommand(OpenTeamWindow);
            DeletePostCommand = new RelayCommand(DeletePost);
            DeleteCommentCommand = new RelayCommand(DeleteComment);
            AddCommentCommand = new RelayCommand(OpenCreateComment);
            CreateCommentCommand = new RelayCommand(CreateComment);
            LoadPostCommand = new RelayCommand(LoadPostById);
            CreatePostCommand = new RelayCommand(CreateNewPost);
            DeleteTeamCommand = new RelayCommand(DeleteTeam);

            ActivateTeamCommand = new RelayCommand(ActivateTeam);
            CreateTeamCommand = new RelayCommand(OpenCreateTeamWindow);
            RefreshTeamsWindowCommand = new RelayCommand(RefreshTeams);

            mediator.Register<ActiveTeamMessage>(GetTeam);
            mediator.Register<ActiveUserMessage>(GetUserAndTeams);
            mediator.Register<ActivePostMessage>(GetPost);
        }

        private void DeleteTeam(object obj)
        {
            Guid id = (Guid)obj;
            LoadTeamById(id);

            if(VerifyDelete()) RemoveTeam();

            mediator.Send(new ActiveUserMessage { Id = User.Id });
        }
        private void OpenTeamWindow(object obj)
        {
            TeamWindow TeamWindow = new TeamWindow();
            TeamWindow.Show();

            mediator.Send(new ActiveTeamMessage { Id = Team.Id });
        }

        private void GetTeam(ActiveTeamMessage obj)
        {
            ActivateTeam(obj.Id);
        }

        private void GetPost(ActivePostMessage obj)
        {
            LoadPostById(obj.Id);
        }

        private void OpenCreateComment(object obj)
        {
            Guid id = (Guid)obj;

            CreateCommentWindow CreateCommentWindow = new CreateCommentWindow();
            CreateCommentWindow.Show();

            mediator.Send(new ActiveTeamMessage { Id = Team.Id });
            mediator.Send(new ActiveUserMessage { Id = User.Id });
            mediator.Send(new ActivePostMessage { Id = id });
        }

        private void CreateNewPost(object obj)
        {
            Post.Author = User;
            Post.Time = DateTime.Now;
            if (string.IsNullOrEmpty(Post.Title) ||
                string.IsNullOrEmpty(Post.MessageContent)){
                infoText = "Title and Content must not be empty";
            }
            else
            {
                CreatePost();
                AddPost();
                object ob = (Team.Id) as object;
                ActivateTeam(ob);
                infoText = "Post successfully created";
            }
        }

        private void ActivateTeam(object obj)
        {
            Guid t = (Guid)obj;
            LoadTeamById(t);
            LoadPostsByTeam(Team.Id); 
        }

        private void RefreshTeams(object obj)
        {
            try
            {
                mediator.Send(new ActiveTeamMessage { Id = Team.Id });
                mediator.Send(new ActiveUserMessage { Id = User.Id });
            }
            catch { }
            
        }

        private void OpenCreateTeamWindow(object obj)
        {

            CreateTeamWindow NewTeamWindow = new CreateTeamWindow();
            NewTeamWindow.Show();

            mediator.Send(new ActiveUserMessage { Id = User.Id });
        }
        private void GetUserAndTeams(ActiveUserMessage obj)
        {
            LoadUserById(obj.Id);
            LoadTeamsByUserId(obj.Id);
        }

        public void LoadTeamsByUserId(Guid id)
        {
            TeamsList = userRepository.GetTeamsByUser(id);
        }
        public void LoadUserById(Guid id)
        {
            User = userRepository.GetById(id);
        }
        public void LoadTeamById(Guid id)
        {
            Team = teamRepository.GetById(id);
        }
        public void CreatePost()
        {
            Post = teamRepository.CreatePost(Post, User.Id);
        }
        public void AddPost()
        {
            Team = teamRepository.AddPost(Team.Id, Post.Id);
        }
        public void LoadPostsByTeam(Guid id)
        {
            PostsList = postRepository.GetPostsByTeam(id);
        }
        public void LoadPostById(object obj)
        {
            Guid id = (Guid)obj;
            Post = postRepository.GetById(id);
        }
        public void CreateComment(object obj)
        {
            Comment.Time = DateTime.Now;

            Comment = postRepository.CreateComment(Comment, User.Id);
            Post = postRepository.AddComment(Post.Id, Comment.Id);

            mediator.Send(new ActiveTeamMessage { Id = Team.Id });
            infoText = "Comment successfully created";
        }
        public void LoadCommentById(Guid id)
        {
            Comment = commentRepository.GetById(id);
        }
        public void DeleteComment(object obj)
        {
            CommentModel c = (CommentModel)obj;
            if (c.Author.Id == User.Id)
            {
                if (VerifyDelete())
                {
                    commentRepository.DeleteComment(c.Id);
                }
                mediator.Send(new ActiveTeamMessage { Id = Team.Id });
            }
            else
            {
                MessageBoxResult messageBox = System.Windows.MessageBox.Show("You must be the author be allowed to delete it.", "Author error", System.Windows.MessageBoxButton.OK);
            }
        }
        public void DeletePost(object obj)
        {
            PostModel p = (PostModel)obj;
            if (p.Author.Id == User.Id)
            {
                if (VerifyDelete())
                {
                    postRepository.DeletePost(p.Id);
                }
                mediator.Send(new ActiveTeamMessage { Id = Team.Id });
            }
            else
            {
                MessageBoxResult messageBox = System.Windows.MessageBox.Show("You must be the author to be allowed to delete.", "Author error", System.Windows.MessageBoxButton.OK);
            }
        }
        public void RemoveTeam()
        {
            teamRepository.RemoveTeam(Team.Id);
            mediator.Send(new ActiveUserMessage { Id = User.Id });
        }
        private bool VerifyDelete()
        {
            MessageBoxResult messageBox = System.Windows.MessageBox.Show("Are you sure you want to delete this?", "Delete", System.Windows.MessageBoxButton.YesNo);
            if (messageBox == MessageBoxResult.Yes)
            {
                return true;
            }
            return false;
        }
    }
}
